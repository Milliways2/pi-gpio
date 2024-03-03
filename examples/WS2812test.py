#! /usr/bin/env python3
import pi_gpio as GPIO
import time

#2023-12-22
# Configure the number of WS2812 LEDs.
# NUM_LEDS = 5
# PIN_NUM = 22
# brightness = 0.2
tx =b'0'
# rst = [b'0']*400
# rst = bytearray(40)
# rst = b'0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'
rst = b'0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'
# 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
# 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

# WSLOOKUP = {0: b'963013116800',
#         1: b'963013116912',
#         2: b'963013231488',
#         3: b'963013231600',,\n
#         4: b'963130557312',
#         5: b'963130557424',
#         6: b'963130672000',
#         7: b'963130672112',
#         8: b'1083272201088',
#         9: b'1083272201200',
#         10: b'1083272315776',
#         11: b'1083272315888',
#         12: b'1083389641600',
#         13: b'1083389641712',
#         14: b'1083389756288',
#         15: b'1083389756400'}


WSLOOKUP = {0: b'\xe08\x0e\x03\x80',
        1: b'\xe08\x0e\x03\xf0',
        2: b'\xe08\x0f\xc3\x80',
        3: b'\xe08\x0f\xc3\xf0',
        4: b'\xe0?\x0e\x03\x80',
        5: b'\xe0?\x0e\x03\xf0',
        6: b'\xe0?\x0f\xc3\x80',
        7: b'\xe0?\x0f\xc3\xf0',
        8: b'\xfc8\x0e\x03\x80',
        9: b'\xfc8\x0e\x03\xf0',
        10: b'\xfc8\x0f\xc3\x80',
        11: b'\xfc8\x0f\xc3\xf0',
        12: b'\xfc?\x0e\x03\x80',
        13: b'\xfc?\x0e\x03\xf0',
        14: b'\xfc?\x0f\xc3\x80',
        15: b'\xfc?\x0f\xc3\xf0'}

controller = 0  #SPI0
channel = 0     #.0
mode = 0
bits = 8
speed = 8000000

ret = GPIO.spiOpen( controller,  channel,  speed,  mode) #SPI controller, channel, speed, mode
if (ret < 0) :
    print("Unable to open /dev/spidev /dev/spidev{:d}.{:d}".format(controller, channel))
    exit(-1)

BLACK = (0, 0, 0)
RED = (255, 0, 0)
YELLOW = (255, 150, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
PURPLE = (180, 0, 255)
WHITE = (255, 255, 255)
COLORS = (BLACK, RED, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE)

# GPIO.spiDataRW (channel, rst, len(rst))

# print(f"tx = {tx}")
# print(f"rst = {rst}")
# print(len(rst))

def col2l(col):
    """
    convert RGB colour to byte pattern
    """
    r, g, b = col   # print(f"RGB = {r}, {g}, {b}")
    return WSLOOKUP[r>>4] + WSLOOKUP[r&0xf] + WSLOOKUP[g>>4] + WSLOOKUP[g&0xf]+ WSLOOKUP[b>>4] + WSLOOKUP[b&0xf]  # print(f"cl = {cl}")
#     return WSLOOKUP[g>>4] + WSLOOKUP[g&0xf] + WSLOOKUP[r>>4] + WSLOOKUP[r&0xf]+ WSLOOKUP[b>>4] + WSLOOKUP[b&0xf]  # print(f"cl = {cl}")

def pixels_set(colour):
    global tx
    tx = rst + col2l(colour) + rst 
    print(f"tx = {tx}")
    print(len(tx))
    print(f"rst = {rst}")
    print(len(rst))
    GPIO.spiDataRW (channel, tx, len(tx))

def pixels_set2(c1, c2):
    global tx
    tx = rst + col2l(c1) + col2l(c2) + rst 
    print(f"tx = {tx}")
    print(len(tx))
    GPIO.spiDataRW (channel, tx, len(tx))

def c2l(col):
    global tx
    r, g, b = col   # print(f"RGB = {r}, {g}, {b}")
    cl = WSLOOKUP[r>>4] + WSLOOKUP[r&0xf] + WSLOOKUP[g>>4] + WSLOOKUP[g&0xf]+ WSLOOKUP[b>>4] + WSLOOKUP[b&0xf]  # print(f"cl = {cl}")
    tx = cl
        
def pixels_set_orig(colour):
    c2l(colour)
    GPIO.spiDataRW (channel, rst, len(rst))
    GPIO.spiDataRW (channel, tx, len(tx))
    GPIO.spiDataRW (channel, rst, len(rst))

col = col2l(RED)
print(f"col= {col}")

##########################################################################
# def pixels_show():
#     dimmer_ar = array.array("I", [0 for _ in range(NUM_LEDS)])
#     for i,c in enumerate(ar):
#         r = int(((c >> 8) & 0xFF) * brightness)
#         g = int(((c >> 16) & 0xFF) * brightness)
#         b = int((c & 0xFF) * brightness)
#         dimmer_ar[i] = (g<<16) + (r<<8) + b
#     sm.put(dimmer_ar, 8)
#     time.sleep_ms(10)
# 
# def pixels_set(i, color):
#     ar[i] = (color[1]<<16) + (color[0]<<8) + color[2]
# 
# def pixels_fill(color):
#     for i in range(len(ar)):
#         pixels_set(i, color)
# 
# def color_chase(color, wait):
#     for i in range(NUM_LEDS):
#         pixels_set(i, color)
#         time.sleep(wait)
#         pixels_show()
#     time.sleep(0.2)
#  
# def wheel(pos):
#     # Input a value 0 to 255 to get a color value.
#     # The colours are a transition r - g - b - back to r.
#     if pos < 0 or pos > 255:
#         return (0, 0, 0)
#     if pos < 85:
#         return (255 - pos * 3, pos * 3, 0)
#     if pos < 170:
#         pos -= 85
#         return (0, 255 - pos * 3, pos * 3)
#     pos -= 170
#     return (pos * 3, 0, 255 - pos * 3)
#  
#  
# def rainbow_cycle(wait):
#     for j in range(255):
#         for i in range(NUM_LEDS):
#             rc_index = (i * 256 // NUM_LEDS) + j
#             pixels_set(i, wheel(rc_index & 255))
#         pixels_show()
#         time.sleep(wait)

pixels_set2(YELLOW, GREEN)
# c2l(RED)
# print(f"tx = {tx}")
# print(len(tx))
# print(len(WSLOOKUP[1]))

# pixels_set(YELLOW)
# pixels_set(GREEN)

# pixels_set(RED)
# pixels_set(RED)
# pixels_set(RED)
# pixels_set(RED)

# print(f"tx = {tx}")
# print(f"rst = {rst}")
# print(len(rst))
# GPIO.spiDataRW (channel, rst, len(rst))
# print(f"rst = {rst}")
# GPIO.spiDataRW (channel, tx, len(tx))
# GPIO.spiDataRW (channel, rst, len(rst))
# time.sleep(1)

# print("fills")
# for color in COLORS:       
#     pixels_fill(color)
#     pixels_show()
#     time.sleep(0.2)

# print("chases")
# for color in COLORS:       
# #     color_chase(color, 0.01)
#     color_chase(color, 0.05)

# print("rainbow")
# rainbow_cycle(0)
