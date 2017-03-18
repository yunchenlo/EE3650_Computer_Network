ans = 15;
plen=length(packet);
M=[packet zeros(1,clen-1)]; %add 0's
mlen=length(M);
C=[1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1];
clen=length(C);

for i = 1: plen
    T = zeros(1, mlen);
    T(i) = 1;       
    temp = T(1:clen);
    for i=1:(length(T)-clen)
        if (temp(1) == 1)
            output = xor(temp,C);
        else
            output = temp;
        end
        temp = [output(2:clen) T(clen+i)];%shifting
    end
    if (temp(1) == 1)
        temp = xor(temp,C);   
    end
    if (length(find(temp)) < ans)
        ans = length(find(temp));
        error = temp;
        index = i;
    end
end

error = [zeros(1, mlen - length(error)) error];
error(index) = 1;

