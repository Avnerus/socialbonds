b = Buffer.alloc(s,2048,1);
(
var index = 0;
o = OSCFunc({ arg msg;
  	msg[1];
	b.setn(index,msg[1]);
	index = index +1;
	if (index == 2048, {index = 0})
}, '/eeg', nil);
)

(
SynthDef(\oscplayer,{ arg out = 0, bufnum;
	var signal;
	signal = Osc.ar(bufnum, MouseX.kr(60,300)); // mouse x to control pitch
	Out.ar(out, signal ! 2)
}).add
)

a = Synth(\oscplayer, [\bufnum, b.bufnum])