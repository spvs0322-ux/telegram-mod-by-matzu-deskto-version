import sys, os

def resolve(arch):
    if sys.platform == 'darwin':
        os.environ['QT'] = '6.11.1'
    elif sys.platform == 'win32':
        if 'qt5' in sys.argv:
            print('Choosing Qt 5.')
            os.environ['QT'] = '5.15.19'
        else:
            print('Choosing Qt 6.')
            os.environ['QT'] = '6.11.1'
    return True
