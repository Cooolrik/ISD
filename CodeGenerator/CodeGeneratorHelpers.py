import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR

def write_lines_to_file( path , lines ):
    # make into one long string
    new_text = ''
    for line in lines:
        new_text += line + '\n'

    # if the file already exists, check if we have an update
    if os.path.exists(path):
        # try reading in the current file if one exists
        with open(path,'r') as f:
            existing_text = f.read()
            f.close()

        #if no difference was found, return
        if new_text == existing_text:
            return

        # if a difference was found, remove the old file
        os.chmod(path, S_IWUSR)
        os.remove( path ) 

    # we should write to the new file
    with open(path,'w') as f:
        f.write(new_text)
        f.close()

    # change mode of file to readonly
    os.chmod(path, S_IRUSR|S_IRGRP|S_IROTH)