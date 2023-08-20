The compressed images if they are gray are around 68% smaller than their original bmp file,
making them around 32% of their original size. For colored images, they are around 11% smaller,
making them around 89% of their original size. 

The total time it takes for the lion.bmp file to compress if in color is around 0.016618 seconds,
where fork() is used to parallelize the readings of the pixels. For gray, the time it takes to 
read and store the pixel data into an array as a gray color value instead takes around 0.009384 seconds.

My functions to retrieve the Huffman codes from my tree is an implementation found online at:
    https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
My implementation for creating the min heap and the huffman tree itself, however, is not from online. 