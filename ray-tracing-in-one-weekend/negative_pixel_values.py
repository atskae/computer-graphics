'''
I don't understand how negative pixel values in PPM files are rendered,
so I'm writing a program to display some...
'''

IMAGE_WIDTH = 255
IMAGE_HEIGHT = 255 * 2 # Set to range -255 to 255
MAX_COLOR_VALUE = 255


def one_channel():
    """Go from negative to positive in one color channel"""
    min_channel_value = -MAX_COLOR_VALUE
    max_channel_value = MAX_COLOR_VALUE
    print(f"Min channel value: {min_channel_value}")
    print(f"max channel value: {max_channel_value}")
    
    image_file_name = "negative_pixel_values.ppm"
    with open(image_file_name, "w") as image_file:
        image_file.write("P3\n")
        image_file.write(f"{IMAGE_WIDTH} {IMAGE_HEIGHT}\n")
        image_file.write(f"{MAX_COLOR_VALUE}\n")
        for row in range(min_channel_value, max_channel_value):
            if row == 0:
                # Draw a unique line in the center
                color = f"255 150 0\n"
            elif row == -100 or row == -10 or (row % MAX_COLOR_VALUE == 0):
                # Draw a white line at sudden color changes
                color = f"255 255 255\n"
            else:
                # Draw blue line based on row
                color = f"0 0 {row}\n"

            # Render line
            for col in range(IMAGE_WIDTH):
                    image_file.write(color)
    
    print(f"Image generated: {image_file_name}")

def two_channels():
    """Change values in two color channels"""
    image_file_name = "negative_pixel_values_two_channels.ppm"
    with open(image_file_name, "w") as image_file:
        image_file.write("P3\n")
        image_file.write(f"{IMAGE_WIDTH} {IMAGE_HEIGHT}\n")
        image_file.write(f"{MAX_COLOR_VALUE}\n")
        for row in range(-1*MAX_COLOR_VALUE, MAX_COLOR_VALUE):
            if row == 0:
                # Draw a unique line in the center
                color = f"0 255 0\n"
            elif abs(row) == 100 or abs(row) == 10:
                # Draw a white line at sudden color changes
                color = f"255 255 255\n"
            else:
                # Draw a line based on row
                color = f"{-row} 0 {row}\n"

            # Render line
            for col in range(IMAGE_WIDTH):
                    image_file.write(color)
    
    print(f"Image generated: {image_file_name}")


def main():
    one_channel()
    two_channels()


if __name__ == "__main__":
    main()
