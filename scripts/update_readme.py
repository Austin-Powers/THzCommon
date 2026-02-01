# This script is used to automatically generate the contents of the README.md
# from the file and doxygen comments inside the project
#
# The generation is limited to the ## Details section that should be located
# at the bottom of the README.md, everything after that section will be deleted
# by the script

import os
import sys

def getProjectDir() -> str:
    '''Returns the project directory, which is either second command line argument given to the script
    or the current working directory
    '''
    if(len(sys.argv) > 1):
        return sys.argv[1]
    return os.getcwd()

def getReadmePath() -> str:
    '''Returns the path of the README.md.
    '''
    return os.path.join(getProjectDir(), 'README.md')

def loadReadme() -> str:
    '''Loads the current content of the README.md.
    '''
    result = ''
    with open(getReadmePath()) as readmefile:
        result = readmefile.read()
    return result

def saveReadme(newContent: str):
    '''Saves the new content of the README.md.
    '''
    with open(getReadmePath(), 'w') as readmefile:
        readmefile.write(newContent)

def listLibraryIncludeSubdirs() -> dict[str, str]:
    '''Returns a list of all subdirectories of the main include directory.
    '''
    result = {}
    includeDir = os.path.join(getProjectDir(), 'include')
    projectDir = ''
    for root, dirs, files in os.walk(includeDir):
        t = os.path.split(root)
        if(t[0] == includeDir):
            projectDir = root
        elif(t[0] == projectDir):
            result[t[1]] = root
    return result

def compileInfoOfFile(root: str, filename: str) -> str:
    '''Compiles the information for the given file.
    '''
    result = ''
    parsingBrief = False
    docString = ''
    with open(os.path.join(root, filename)) as file:
        for rawLine in file:
            line = rawLine.replace('\r', '').replace('\n', '')
            if(line.startswith('/// @brief ')):
                docString = line[11:]
                parsingBrief =  True
            elif(line.startswith('/// @')):
                parsingBrief = False
            elif(line.startswith('///')):
                if(parsingBrief):
                    docString = docString + (line[3:])
            elif(line.startswith('template')):
                pass
            elif(line.startswith('class ')):
                # TODO umstellen auf string format
                entry = '- __' + line.split(' ')[1] + '__ `class` `<' + filename + '>` ' + docString
                result = result + entry + '\n'
                docString = ''
            elif(line.startswith('enum class ')):
                entry = '- __' + line.split(' ')[2] + '__ `enum` `<' + filename + '>` ' + docString
                result = result + entry + '\n'
                docString = ''
            elif(line.startswith('struct ')):
                entry = '- __' + line.split(' ')[1] + '__ `struct` `<' + filename + '>` ' + docString
                result = result + entry + '\n'
                docString = ''
            elif(line.startswith('using ') or line.startswith('#define ')):
                if(docString != ''):
                    entry = '- __' + line.split(' ')[1] + '__ `definition` `<' + filename + '>` ' + docString
                    result = result + entry + '\n'
                    docString = ''
            elif(line.startswith('concept ')):
                entry = '- __' + line.split(' ')[1] + '__ `concept` `<' + filename + '>` ' + docString
                result = result + entry + '\n'
                docString = ''
            elif(docString != ''):
                print(line.strip() + ' of file ' + filename + ' is unknown')
                docString = ''
    return result

def compileInfoOfSubdir(dirpath: str) -> str:
    '''Compiles the information for the given subdirectory.
    '''
    result = ''
    for root, dirs, files in os.walk(dirpath):
        for file in files:
            filecontent = compileInfoOfFile(root, file)
            if(len(filecontent) > 0):
                result += (filecontent + '  \n')

    result = result + '\n'
    return result

# we split of the part of the content that does not change
content = loadReadme().split('## Details')[0]
content = content + '''## Details
Below is more detailed information about the contents of each subdirectory.

'''

subDirs = listLibraryIncludeSubdirs()
for key in subDirs:
    paragraph = '### ' + key.capitalize() + '\n'
    paragraph = paragraph + compileInfoOfSubdir(subDirs[key])
    content = content + paragraph

saveReadme(content)
