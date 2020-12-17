from PIL import Image
#import serial

im = Image.open("mario_run_3.png")
width, height = im.size

output = "byte rs[] = {"

for y in range(height):
    for x in range(width):
        if im.getpixel((x, y))[0] == 255 and im.getpixel((x, y))[1] == 255 and im.getpixel((x, y))[2] == 255:
            output +=  "B00000000" + ", "
        else:
            output +=  "B" + '{0:08b}'.format(im.getpixel((x, y))[0]) + ", "
                

output = output[:-2];
output += "};\n"

output += "byte gs[] = {"

for y in range(height):
    for x in range(width):
        if im.getpixel((x, y))[0] == 255 and im.getpixel((x, y))[1] == 255 and im.getpixel((x, y))[2] == 255:
            output +=  "B00000000" + ", "
        else:
            output +=  "B" + '{0:08b}'.format(im.getpixel((x, y))[1]) + ", "
                

output = output[:-2];
output += "};\n"

output += "byte bs[] = {"

for y in range(height):
    for x in range(width):
        if im.getpixel((x, y))[0] == 255 and im.getpixel((x, y))[1] == 255 and im.getpixel((x, y))[2] == 255:
            output +=  "B00000000" + ", "
        else:
            output +=  "B" + '{0:08b}'.format(im.getpixel((x, y))[2]) + ", "
                

output = output[:-2];
output += "};\n"




print(output);


#ser = serial.Serial('/dev/tty.usbserial', 9600)
#while True:
#    print ser.readline()
#    ser.write('5')
