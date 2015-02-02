% fileOut.m 
% Writes the pixel values of the quantized image as .txt tab delimited file

function [ ] = fileOut(imName, Q)

str = strcat(imName,'_output.txt');
dlmwrite(str, Q, 'delimiter', ' ');

end