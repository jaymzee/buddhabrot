# buddhabrot
**mandelbrot**      render traditional mandelbrot  
**buddhabrot**      render buddhabrot (C implementation, 2x to 3x faster)  
**buddhabrotpp**    render buddhabrot (C++ implementation using complex.h)  
**buddhabrotmt**    render buddhabrot (multithreaded implementation, fastest)  
**mplshow**         display and save color PNG image from rendered image data
                    (Python 3, numpy, Matplotlib.pyplot)  
**pilshow**         display and save grayscale PNG image from rendered image..
                    data (Python 3, numpy, Pillow)  

![buddhabrot image](doc/img/bb-i100k-s100M.png)

#### render image to intermediate file format then display and convert image to png
```
buddhabrot -w1024 -h1280 -i1000 -s10000000 images/bb-i1k-s10M
mplshow -t -cviridis images/bb-i1k-s10M
pilshow -t images/bb-ilk-s10M
```
