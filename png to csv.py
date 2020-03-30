from PIL import Image
import numpy as np
print()

img=Image.open(r"path to file")
imgarray=np.array(img)

print(np.size(imgarray))

print()

for row in imgarray:
    outputstring=""
    for column in row:
        valuestring=""
        for value in column:
            if value < 10:
                valuestring+="00"+str(value)
            elif value < 100:
                valuestring+="0"+str(value)
            else:
                valuestring+=str(value)
        outputstring+=valuestring+";"
    print(outputstring[:-1]) #excluding the last letter, because its a ;)
    

f = open('filename.txt','wb')
f.write(outputstring[:-1].encode())
f.close()