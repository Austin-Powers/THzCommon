# This script is used to automatically extract TODO comments from the
# The source files in a project and create an HTML file from it

import os
import sys

def getProjectDir() -> str:
    '''Returns the project directory, which is either second command line argument given to the script
    or the current working directory
    '''
    if(len(sys.argv) > 1):
        return sys.argv[1]
    return os.getcwd()

def finallizeContentForFile(content: str, filename: str) -> str:
    '''Performs the last checks before the content for a file is handed back to the caller.'''
    if (len(content) == 0):
        return ''
    return f'''<h3 id="{filename}">{filename}</h3>
    {content}
<hr />
'''

def createContentForComment(filename: str, startingLine: int, comment: str) -> str:
    print(f'{filename}L{startingLine}: {comment}')
    return f'<b>Z{startingLine}:</b> {comment}'

def createContentForFile(root: str, filename: str, todoStart: str, commentStart: str) -> str:
    '''Creates the content for a specific file'''
    content = ''
    with open(os.path.join(root, filename)) as file:
        lineNumber = 1
        startingLine = 1
        comment = ''
        for rawline in file:
            line = rawline.strip()
            if(line.startswith(todoStart)):
                if(len(comment) != 0):
                    # finalize last comment
                    content = createContentForComment(filename, startingLine, comment)
                startingLine = lineNumber
                comment = line[len(todoStart):]
            elif(line.startswith(commentStart)):
                if(len(comment) != 0):
                    comment = comment + ' ' + line[len(commentStart):]
            else:
                if(len(comment) != 0):
                    content = createContentForComment(filename, startingLine, comment)
                    comment = ''
            lineNumber += 1
    return finallizeContentForFile(content, filename)

def createContent() ->  str:
    '''Creates the content for the todo list.'''
    content = ''
    for root, dirs, files in os.walk(getProjectDir()):
        if((root.find('build') != -1) or (root.find('.git') != -1)):
            # remove files from build directory
            continue
        for file in files:
            if(file.endswith(( '.cpp', '.hpp'))):
                content = content + createContentForFile(root, file, '// TODO ', '// ')
            if(file.endswith(( '.py'))):
                content = content + createContentForFile(root, file, '# TODO ', '# ')
    return content

def getProjectName() -> str:
    '''Returns the name of the project'''
    return os.path.split(getProjectDir())[1]

def addHeadAndTail(content: str) -> str:
    '''Adds head and tail to the html content.'''
    projectName = getProjectName()
    return f'''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>TODOs {projectName}</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body{{
            max-width:650px;
            margin:40px auto;
            padding:0 10px;
            font:18px/1.5 -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, "Noto Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Noto Color Emoji";
            color:#444
        }}
        h1,h2,h3{{
            line-height:1.2
        }}
        @media (prefers-color-scheme: dark){{
            body{{color:#c9d1d9;background:#0d1117}}
            a:link{{color:#58a6ff}}
            a:visited{{color:#8e96f0}}
        }}
    </style>
</head>
<body>
    <h1 id="todo-title">TODOs {projectName}</h1>
    {content}
    <p>Licensed under <a href="https://creativecommons.org/publicdomain/zero/1.0/">CC0</a>.</p>
</body>
</html>
'''

def writeTodoFile(content: str):
    '''Writes the content of the todo file.'''
    with open(os.path.join(getProjectDir(), 'todos.html'), 'w') as file:
        file.write(content)

print(f'Extracting TODOS for {getProjectName()}')
writeTodoFile(addHeadAndTail(createContent()))
