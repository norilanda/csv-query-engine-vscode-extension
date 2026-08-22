import * as vscode from 'vscode';
import * as path from 'path';
import { exec } from 'child_process';

const queryCsvBackendFileName = 'backend-query-csv';

export function activate(context: vscode.ExtensionContext) {
    console.log('Extension "csv-query-extension" is now active!');

    const disposable = vscode.commands.registerCommand('csv-query-extension.queryCsv', async () => {
        
        // 1. Get the active editor
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showWarningMessage('No active file open. Please open a CSV file first.');
            return;
        }

        // 2. Validate input file is a CSV
        const inputFilePath = editor.document.uri.fsPath;
        const fileExtension = path.extname(inputFilePath).toLowerCase();
        
        if (fileExtension !== '.csv') {
            vscode.window.showWarningMessage(`The active file is not a CSV. Found: ${fileExtension || 'no extension'}`);
            return;
        }

        // 3. Prompt user for SQL query
        const userQuery = await vscode.window.showInputBox({
            prompt: 'Enter your query for this CSV',
            placeHolder: 'e.g., SELECT name, age WHERE age > 25 LIMIT 10',
            ignoreFocusOut: true
        });

        if (!userQuery) {
            return;
        }

        // 4. Determine output file path (e.g., "/path/data.csv" -> "/path/data_result.csv")
        const parsedPath = path.parse(inputFilePath);
        const outputFilePath = path.join(parsedPath.dir, `${parsedPath.name}_result.csv`);

        // 5. Find the backend binary (handles Windows vs Linux/macOS)
        const isWindows = process.platform === 'win32';
        const binaryName = isWindows ? queryCsvBackendFileName +'.exe' : queryCsvBackendFileName;
        const cppExecutable = path.join(context.extensionPath, 'backend/x64/Release', binaryName);

        // 6. Build the CLI command
        // Escaping double quotes inside the query so the CLI shell interprets it correctly
        const escapedQuery = userQuery.replace(/"/g, '\\"');
        const command = `"${cppExecutable}" "${inputFilePath}" "${outputFilePath}" "${escapedQuery}"`;

        // 7. Execute backend with a progress notification
        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'Executing CSV query...',
            cancellable: false
        }, () => {
            return new Promise<void>((resolve) => {
                exec(command, (error, stdout, stderr) => {
                    if (error) {
                        vscode.window.showErrorMessage(`Query Failed: ${stderr.trim() || error.message}`);
                        resolve();
                        return;
                    }

                    if (stderr) {
                        vscode.window.showWarningMessage(`Backend Warning: ${stderr.trim()}`);
                    }

                    // Success!
                    vscode.window.showInformationMessage(`${stdout.trim()} !`);
                    resolve();
                });
            });
        });
    });

    context.subscriptions.push(disposable);
}

export function deactivate() {}