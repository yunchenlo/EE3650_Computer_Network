	plen=length(packet); %the length of the packet
	
	C=[1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1]; %the crc
	
	clen=length(C); %the length of the crc
	
	M=[packet zeros(1,clen-1)]; %add 0's
	mlen=length(M);
	R=packet(1:clen);

	for i=1:(mlen-clen+1)
	    if (R(1,1)==1)
	        R=xor(R,C);
	    end
	    if (i<plen)                         %shifting
	        R=[R(2:clen) M(1,clen+i)];
	    end
	end

	codepacket=[M(1:plen) (M(plen+1:mlen) + R(2:clen))];