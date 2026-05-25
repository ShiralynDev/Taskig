import * as vscode from 'vscode';
import { spawn } from 'child_process';

export function activate(context: vscode.ExtensionContext) {

    const provider = new TaskigListProvider();

    context.subscriptions.push(
        vscode.window.registerTreeDataProvider(
            "taskigView",
            provider
        )
    );

    const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
    if (!workspaceFolder) return;

    const disposable = vscode.workspace.onDidSaveTextDocument((document) => {

        if (!document.uri.fsPath.startsWith(workspaceFolder.uri.fsPath)) {
            return;
        }

        const child = spawn(
            '/usr/local/bin/Taskig',
            ['scan'],
            {
                cwd: workspaceFolder.uri.fsPath
            }
        );

        child.on('close', () => {
            provider.refresh();
        });
    });

    context.subscriptions.push(disposable);

}

export function deactivate() {}

class TaskigListProvider implements vscode.TreeDataProvider<string> {

    private _onDidChangeTreeData = new vscode.EventEmitter<string | undefined | void>();
    readonly onDidChangeTreeData = this._onDidChangeTreeData.event;
    private taskigTasks: string[] = [];

    constructor() {
        this.loadTaskigTasks();
    }

    refresh() {
        this.loadTaskigTasks();
    }

    getTreeItem(element: string): vscode.TreeItem {
        const parsed = parseTask(element);

        const item = new vscode.TreeItem(
            parsed?.description ?? element,
            vscode.TreeItemCollapsibleState.None
        );

        if (!parsed) {
            return item;
        }

        const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
        if (!workspaceFolder) return item;

        const uri = vscode.Uri.joinPath(workspaceFolder.uri, parsed.path);

        item.description = `${parsed.path}:${parsed.line}`;

        item.command = { // [taskig] fix command
            command: 'vscode.open',
            title: 'Open Task',
            arguments: [
                uri,
                {
                    selection: new vscode.Range(
                        new vscode.Position(parsed.line - 1, 0),
                        new vscode.Position(parsed.line - 1, 9999)
                    )
                }
            ]
        };

        console.log("Opening:", uri.fsPath, parsed);

        return item;
    }

    getChildren(): Thenable<string[]> {
        return Promise.resolve(this.taskigTasks);
    }

    private async loadTaskigTasks() {
        const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
        if (!workspaceFolder) {
            return;
        }

        return new Promise((resolve, reject) => {

            const taskig = spawn(
                '/usr/local/bin/Taskig',
                [
                    'list'
                ],
                {
                    cwd: workspaceFolder.uri.fsPath
                }
            );

            let stdout = '';
            let stderr = '';

            taskig.stdout.on('data', data => {
                stdout += data.toString();
            })

            taskig.stderr.on('data', data => {
                stderr += data.toString();
            })

            taskig.on('error', err => {
                vscode.window.showErrorMessage(`Failed to run Taskig: ${err.message}`);
            });

            taskig.on('close', code => {
                if (code !== 0) {
                    reject(stderr);
                    return;
                }

                const tasks = stdout // [taskig] display amount of tasks in a better way
                    .split('\n')
                    .map(x => x.trim())
                    .filter(Boolean);

                this.taskigTasks = tasks; // [taskig] reorder so desc is shown first
                this._onDidChangeTreeData.fire();
            });

        })
    }
}

function parseTask(line: string) {
    const match = line.match(/^"?(.+?)"?:([0-9]+)\s+\[(.*)\]$/);
    if (!match) return null;

    const [path, lineNumber, description] = match;

    return {
        path,
        line: Number(lineNumber),
        description: description.trim()
    };
}