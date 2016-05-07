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

d = Buffer.alloc(s,2048,1);
(
var index = 0;
o = OSCFunc({ arg msg;
  	msg[1];
	d.setn(index,msg[1]);
	index = index +1;
	if (index == 2048, {index = 0})
}, '/lpp', nil);
)

(
SynthDef(\oscplayer,{ arg out = 0, bufnum;
	var signal;
	signal = Osc.ar(bufnum, MouseX.kr(60,300)); // mouse x to control pitch
	Out.ar(out, signal ! 2)
}).add
)

a = Synth(\oscplayer, [\bufnum, b.bufnum])

(
SynthDef(\lpp, { arg out = 0, bufnum =0;
	var signal;
	signal = PinkNoise.ar(Decay2.ar(Impulse.ar(10,0), 0.0005, 0.2,PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), loop:1)),10.0);
	Out.ar(out, signal ! 2)
}).add
)

c = Synth(\lpp, [\bufnum, d.bufnum])

d.plot()


//{PinkNoise.ar(Decay2.ar(Impulse.ar(10,0,0.2),0.0005,0.2))!2}.play()
//{PinkNoise.ar(Decay2.ar(Impulse.ar(10,0,0.2),0.0005,0.2))!2}.play()