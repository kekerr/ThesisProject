%This script reads an image, adds noise, then applies filters to remove
%noise. Filtered image is then quantized and output to a .txt file to be
%read in C program

% These variables are used to add noise to and quantize the image
imName = 'bird' ;   %Image name
imType = 'jpg' ;    %Image file type
bins = 10;          %Needs to match number of bins in C code
value = 3;          %Averaging box size for salt and pepper noise
variance = 0.05;    %Additive white Gaussian noise
d = 0.05;           %Salt and Pepper noise
m=3;
n=3;
gaussian = 0;       %0 = false
filter = 0;         %0 = false 
noise = 0;          %0 = false

% Read image image
I = imread(imName,imType);

% Add and remove noise from image. Specify gaussian, filter and noise in
% variables above to choose type of noise/add filter
if noise == 1 
    if gaussian == 1
        I = imnoise(I, 'gaussian', 0, variance); %default zero 
        if filter == 1
            I = medfilt2(I, [m n]);
        end
    else
        I = imnoise(I, 'salt & pepper', d);
        if filter == 1
            I = medfilt2(I, [m n]);
        end 
    end
end 

% Quantize image
I = double(I);
Q = quantize(I, bins);
D = mat2gray(Q);

imshow(D)

fileOut(imName, Q);