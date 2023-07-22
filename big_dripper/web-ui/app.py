import io
from distutils.log import debug
from fileinput import filename

import PIL.ImageQt
from flask import *
import os
from pathlib import Path, PurePath

from werkzeug.utils import secure_filename
from werkzeug.datastructures import FileStorage
from PIL import Image, ImageDraw, ImageFont
import time
import numpy as np

from hardware.dripperator_driver import DripperatorDriver
dripperator = DripperatorDriver("/dev/serial0", 17, 6)

app = Flask(__name__)

# create the folders when setting up your app
uploaded_images_path: Path = Path(app.instance_path, 'uploaded_images')
uploaded_images_path.mkdir(exist_ok=True)

all_off = bytes.fromhex('000000000000')
def arr_to_dripperator(arr):
    dripperator_commands = []
    arr = np.flip(arr).astype(int)
    for row in arr:
        r = ""
        for px in row:
            r = r+str(px)
        dripperator_commands.append(bytes.fromhex(bin_str_to_hex(r)))

    return dripperator_commands


def img_to_arr(path):
    im = Image.open(path)
    image_size = im.size
    image_mode = im.mode
    image_info = im.info

    data = np.invert(np.asarray(im))

    #print(image_mode, image_size)
    #print(image_info)

    #print(type(data))
    #print(data.shape)

    data = np.flip(data)

    im.close()

    return data


def bin_str_to_hex(bstr):

    return '%0*X' % ((len(bstr) + 3) // 4, int(bstr, 2))


def print_row(row):
    print("".join(["{:0>8}".format(bin(X)[2:])for X in row]))


@app.route('/')
def main():
    uploaded_images = map(lambda path: path.name, uploaded_images_path.glob('*'))
    return render_template("index.html", uploaded_images=uploaded_images)


@app.route('/uploaded_images/<path:filename>')
def uploaded_images(filename):
    return send_from_directory(uploaded_images_path, filename)


@app.route('/success', methods=['POST'])
def success():
    if request.method == 'POST':
        f = request.files['file']
        palette_image = Image.new('P', (1, 1))  # tiny cuz we don't need any image data
        palette_image.putpalette([255, 255, 255] + [0, 0, 0] * 255)
        black_and_white = Image.open(io.BytesIO(f.read()))\
            .resize((48, 30))\
            .convert('RGB')\
            .quantize(palette=palette_image, dither=Image.NONE)\
            .convert('1')

        path = Path(uploaded_images_path, secure_filename(f.filename)).with_suffix(".bmp")
        black_and_white.save(path)

        data = img_to_arr(path)
        dripperator_commands = arr_to_dripperator(data)

        for drip in dripperator_commands:
            dripperator.display_row(drip)
            print_row(drip)
            time.sleep(0.1)

        driperator.display_row(all_off)
        return render_template("success.html", name=f.filename)


@app.route('/successText', methods=['POST'])
def successText():
    if request.method == 'POST':
        font = ImageFont.truetype(os.path.abspath(os.path.join(os.path.dirname( __file__ ), 'static/fonts/Asai-Analogue.ttf')), 15)
        text = request.form.get("text")
        blank_canvas = Image.open(os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', 'bitmap_images/blank_canvas.bmp')))
        I1 = ImageDraw.Draw(blank_canvas)
        I1.text((1, 7), text, font=font)

        path = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', f'bitmap_images/{text}.bmp'))
        blank_canvas.save(path)
        data = img_to_arr(path)
        dripperator_commands = arr_to_dripperator(data)

        for drip in dripperator_commands:
            dripperator.display_row(drip)
            print_row(drip)
            time.sleep(0.1)

        driperator.display_row(all_off)

        return render_template("successText.html")

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
