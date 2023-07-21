from distutils.log import debug
from fileinput import filename
from flask import *
import os
from werkzeug.utils import secure_filename

app = Flask(__name__)

# create the folders when setting up your app
os.makedirs(os.path.join(app.instance_path, 'uploaded_images'), exist_ok=True)


@app.route('/')
def main():
    return render_template("index.html")


@app.route('/success', methods=['POST'])
def success():
    if request.method == 'POST':
        f = request.files['file']
        f.save(os.path.join(app.instance_path,
               'uploaded_images', secure_filename(f.filename)))
        return render_template("success.html", name=f.filename)


if __name__ == '__main__':
    app.run(debug=True)
