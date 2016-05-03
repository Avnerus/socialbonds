// { SinOsc.ar(200, 0, 0.2) }.scope;
(
	SynthDef("eeg-synth", { arg freq = 440, out = 0;
		Out.ar(out, SinOsc.ar(freq, 0, 0.2));
	}).send(s);
);

g = Synth("eeg-synth");

o = OSCFunc({ arg msg;
//	msg[1].postln;
	g.set("freq", msg);
}, '/eeg', nil);

//o.free