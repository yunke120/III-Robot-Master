from PIL import Image

# 读取图片
image_path = "./ptzleftdown.png"
original_image = Image.open(image_path)

# 旋转图片45°
rotated_image = original_image.rotate(315)

# 保存旋转后的图片
output_path = "./ptzleftdown3.png"
rotated_image.save(output_path)

print("Image rotated and saved successfully.")
