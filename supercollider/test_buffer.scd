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
	var signal, subosc;
	signal = Osc.ar(bufnum, MouseX.kr(60,300)); // mouse x to control pitch
	subosc = {SinOsc.ar(80,1,0.5)};
    Out.ar(0,
        Pan2.ar(subosc + signal)
    )
}).add
)

a = Synth(\oscplayer, [\bufnum, b.bufnum])
a.free();


(
SynthDef(\lppclick, {

    var subosc, subenv, suboutput, clickosc, clickenv, clickoutput;

    subosc = {SinOsc.ar(50,1,0.5)};
    subenv = {Line.ar(1, 0, 0.05, doneAction: 2)};

	clickosc = {LPF.ar(WhiteNoise.ar(1),100,1,0.5)};
    clickenv = {Line.ar(1, 0, 0.02)};

    suboutput = (subosc * subenv);
    clickoutput = (clickosc * clickenv);

    Out.ar(0,
        Pan2.ar(suboutput + clickoutput, 0,5)
    )
}).send(s);
)

(
var index = 0;
o = OSCFunc({ arg msg;
	 Synth(\lppclick)
}, '/lpp', nil);
)

c.free()


d.plot()


//{PinkNoise.ar(Decay2.ar(Impulse.ar(10,0,0.2),0.0005,0.2))!2}.play()
//{PinkNoise.ar(Decay2.ar(Impulse.ar(10,0,0.2),0.0005,0.2))!2}.play()\

/*
d = Buffer.alloc(s,128,1);
(
var index = 0;
o = OSCFunc({ arg msg;
  	msg[1];
	d.setn(index,msg[1]);
	index = index +1;
	if (index == 128, {index = 0})
}, '/lpp', nil);
)

(
SynthDef(\lpp, { arg out = 0, bufnum =0;
	var signal;
    signal = PinkNoise.ar(PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), loop:1));
	//signal = SinOsc.ar(MouseX.kr(60,300),0,PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), loop:1));
	Out.ar(out, signal ! 2)
}).add
)

c = Synth(\lpp, [\bufnum, d.bufnum])
*/