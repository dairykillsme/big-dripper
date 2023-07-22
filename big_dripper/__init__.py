from PIL import Image, ImageDraw, ImageFont
import os

if __name__ == '__main__':
    print("Hello world")

    font = ImageFont.truetype("./fonts/Asai-Analogue.ttf", 15)

    text = 'BIG DRIP'

    document_path = f"{os.getcwd()}/web-ui/instance/uploaded_text/{text}.txt"

    with open(document_path) as f:
        text = f.readline()
        # Open an Image
        img = Image.open(f"{os.getcwd()}/bitmap_images/blank_canvas.bmp")

        # Call draw Method to add 2D graphics in an image
        I1 = ImageDraw.Draw(img)

        # Add Text to an image
        I1.text((1, 10), text, font=font)

        # Display edited image
        # img.show()

        # Save the edited image
        img.save(f"{os.getcwd()}/bitmap_images/{text}.bmp")

