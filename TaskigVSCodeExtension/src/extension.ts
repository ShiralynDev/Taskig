import * as vscode from 'vscode';

export function activate(context: vscode.ExtensionContext) {
    const provider = {
        getTreeItem: (element: string) => {
            return new vscode.TreeItem(element);
        },
        getChildren: () => {
            return Promise.resolve(["Task temp", "Task temp 69"]);
        }
    };

    vscode.window.registerTreeDataProvider(
        "taskigView",
        provider
    );
}

export function deactivate() {}