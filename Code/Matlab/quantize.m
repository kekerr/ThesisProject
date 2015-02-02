%quantize.m
%This script quantizes an image. Need to specify file name/type, number
%of bins and colour type of image.

function [Q] = quantize(A, num)
S=size(A);
n=S(1);
m=S(2);
bin=256/num;

for i=1:n %n 
    for j=1:m
        A(i,j);
        B(i,j) = ((floor(A(i,j)/bin))+1); %what bin it's in
        Q(i,j) = round(((bin/2)+(bin*((B(i,j))-1))));
    end
end

end
