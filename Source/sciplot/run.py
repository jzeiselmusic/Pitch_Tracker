import os
import re

def replace_sciplot(text):
    pattern = r'<sciplot/(.*?)>'
    replaced_text = re.sub(pattern, '"\\1"', text)
    return replaced_text

path = [os.path.join(dp, f) for dp, dn, fn in os.walk(os.path.expanduser("./")) for f in fn]

for filename in path:
    if filename.endswith('.cpp') or filename.endswith('.hpp'):  # Change the extension to the type of files you want to modify
        with open(filename, 'r') as file:
            original_text = file.read()
        
        modified_text = replace_sciplot(original_text)
        
        with open(filename, 'w') as file:
            file.write(modified_text)

print("Replacement complete for all files in the folder.")
