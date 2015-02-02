%fileIn.m 
%Reads the .txt file from the C program and converts to a greyscale image. 


function[R] = fileIn(imName, R)

imName = 'bird_output.txt'

S = dlmread(imName);
R = mat2gray(S);

imshow(R);

end