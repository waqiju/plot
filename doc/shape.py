import math


a = []
for i in range(0, 5):
    degree = 90 - 72 * i
    radians = math.radians(degree)
    radius = 0.5
    a.append((math.cos(radians) * radius, math.sin(radians) * radius))


b = []
for i in range(0, 5):
    degree = 270 - 72 * i
    radians = math.radians(degree)
    radius = 0.3819660112501 * 0.5
    b.append((math.cos(radians) * radius, math.sin(radians) * radius))


for coordinate in a:
    print(f'Vector3({coordinate[0]}f, {coordinate[1]}f, 0),')

print('\n')
for coordinate in b:
    print(f'Vector3({coordinate[0]}f, {coordinate[1]}f, 0),')