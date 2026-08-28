import * as vscode from 'vscode';
import * as path from 'path';
import * as fs from 'fs';
import { execFile } from 'child_process';
import { queryCsvBackendFileName, queryHeaderBackendFileName } from './constants';

const getBackendExecutablePath = (context: vscode.ExtensionContext, backendFileName: string): string => {
    const isWindows = process.platform === 'win32';
    const binaryName = isWindows ? backendFileName + '.exe' : backendFileName;
    return path.join(context.extensionPath, 'backend/x64/Release', binaryName);
};

const verifyCsvFileOpen = async (editor: vscode.TextEditor | undefined): Promise<string | undefined> => {
    let inputFilePath = '';
    
    if (!editor) {
        const enteredFilePath = await vscode.window.showInputBox({
            prompt: 'No active CSV file is open. Enter the full path to a CSV file.',
            placeHolder: 'e.g., C:\\data\\input.csv',
            ignoreFocusOut: true
        });

        if (!enteredFilePath) {
            return;
        }

        const normalizedPath = enteredFilePath
            .trim().replace(/^["']+|["']+$/g, '');

        if (!fs.existsSync(normalizedPath)) {
            vscode.window.showWarningMessage(`File not found: ${normalizedPath}`);
            return;
        }

        inputFilePath = normalizedPath;
    }
    else {
        inputFilePath = editor.document.uri.fsPath;
    }

    const fileExtension = path.extname(inputFilePath).toLowerCase();
    
    if (fileExtension !== '.csv') {
        vscode.window.showWarningMessage(`The active file is not a CSV. Found: ${fileExtension || 'no extension'}`);
        return;
    }
    return inputFilePath;
};

export function activate(context: vscode.ExtensionContext) {
    console.log('Extension "csv-query-extension" is now active!');

    const queryCsvDisposable = vscode.commands.registerCommand('csv-query-extension.queryCsv', async () => {
        
        const editor = vscode.window.activeTextEditor;
        const inputFilePath = await verifyCsvFileOpen(editor);
        if (!inputFilePath) {
            return;
        }

        const userQuery = await vscode.window.showInputBox({
            prompt: 'Enter your query for this CSV',
            placeHolder: 'e.g., SELECT name, age WHERE age > 25 LIMIT 10',
            ignoreFocusOut: true
        });

        if (!userQuery) {
            return;
        }

        const parsedPath = path.parse(inputFilePath);
        const outputFilePath = path.join(parsedPath.dir, `${parsedPath.name}_result.csv`);

        const cppExecutable = getBackendExecutablePath(context, queryCsvBackendFileName);

        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'Executing CSV query...',
            cancellable: false
        }, () => {
            return new Promise<void>((resolve) => {
                execFile(cppExecutable, [inputFilePath, outputFilePath, userQuery], (error, stdout, stderr) => {
                    if (error) {
                        vscode.window.showErrorMessage(`Query Failed: ${stderr.trim() || error.message}`);
                        resolve();
                        return;
                    }

                    if (stderr) {
                        vscode.window.showWarningMessage(`Backend Warning: ${stderr.trim()}`);
                    }

                    vscode.window.showInformationMessage(`${stdout.trim()} !`);
                    resolve();
                });
            });
        });
    });

    const queryHeaderDisposable = vscode.commands.registerCommand('csv-query-extension.queryHeader', async () => {

        const editor = vscode.window.activeTextEditor;
        const inputFilePath = await verifyCsvFileOpen(editor);
        if (!inputFilePath) {
            return;
        }

        const cppExecutable = getBackendExecutablePath(context, queryHeaderBackendFileName);

        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'Querying CSV header...',
            cancellable: false
        }, () => {
            return new Promise<void>((resolve) => {
                execFile(cppExecutable, [inputFilePath], (error, stdout, stderr) => {
                    if (error) {
                        vscode.window.showErrorMessage(`Header Query Failed: ${stderr.trim() || error.message}`);
                        resolve();
                        return;
                    }

                    if (stderr) {
                        vscode.window.showWarningMessage(`Backend Warning: ${stderr.trim()}`);
                    }

                    vscode.window.showInformationMessage(stdout.trim() || 'CSV header query completed.');
                    resolve();
                });
            });
        });
    });

    context.subscriptions.push(queryCsvDisposable, queryHeaderDisposable);
}

export function deactivate() {}