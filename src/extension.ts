// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
import * as vscode from 'vscode';
import { exec } from 'child_process';
import * as path from 'path';

// This method is called when your extension is activated
// Your extension is activated the very first time the command is executed
export function activate(context: vscode.ExtensionContext) {

	// Use the console to output diagnostic information (console.log) and errors (console.error)
	// This line of code will only be executed once when your extension is activated
	console.log('Congratulations, your extension "csv-query-extension" is now active!');

	// The command has been defined in the package.json file
	// Now provide the implementation of the command with registerCommand
	// The commandId parameter must match the command field in package.json
	const disposable = vscode.commands.registerCommand('csv-query-extension.helloWorld', async () => {
		// The code you place here will be executed every time your command is executed
		// Display a message box to the user
        // Get input from user
        const input = await vscode.window.showInputBox({
            prompt: 'Enter a string to process',
            placeHolder: 'Type something...'
        });

        if (!input) {
            vscode.window.showWarningMessage('No input provided');
            return;
        }

		const cppExecutable = path.join(context.extensionPath, 'bin', 'main');

        // Execute C++ program with input
        exec(`"${cppExecutable}" "${input}"`, (error, stdout, stderr) => {
            if (error) {
                vscode.window.showErrorMessage(`Error: ${error.message}`);
                return;
            }
            if (stderr) {
                vscode.window.showErrorMessage(`Stderr: ${stderr}`);
                return;
            }
            
            // Show the output from C++ program
            vscode.window.showInformationMessage(`Result: ${stdout.trim()}`);
        });
	});

	context.subscriptions.push(disposable);
}

// This method is called when your extension is deactivated
export function deactivate() {}
