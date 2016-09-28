var index;
s.doWhenBooted({
	b = Buffer.alloc(s,2048,1);
	(
		SynthDef(\oscplayer,{ arg out = 0, bufnum;
			var signal, subosc;
			//signal = Osc.ar(bufnum, MouseX.kr(60,300)); // mouse x to control pitch
			//	signal = Osc.ar(bufnum, 125.32056);
			signal = Osc.ar(bufnum, 72.653688);
			subosc = {SinOsc.ar(80,1,0.5)};
			Out.ar(0,
				LPF.ar(
					Pan2.ar(subosc + signal),	// the source to be filtered
					2000		// control the filter frequency with a line
					,1,0.6)
			)
		}).send(s);
		s.sync;
		a = Synth(\oscplayer, [\bufnum, b.bufnum]);
	);




	index = 0;
	o = OSCFunc({ arg msg;
		msg[1];
		b.setn(index,msg[1]);
		index = index +1;
		if (index == 2048, {index = 0})
	}, '/eeg', nil);


	(
		SynthDef(\lppclick, {

			var subosc, subenv, suboutput, clickosc, clickenv, clickoutput;

			subosc = {SinOsc.ar(60,1,0.5)};
			subenv = {Line.ar(1, 0, 0.05, doneAction: 2)};

			clickosc = {LPF.ar(WhiteNoise.ar(1),20,1,0)};
			clickenv = {Line.ar(1, 0, 0.01)};

			suboutput = (subosc * subenv);
			clickoutput = (clickosc * clickenv);

			Out.ar(0,
				Pan2.ar(suboutput + clickoutput, 0,5)
			)
		}).send(s);
	);


	o = OSCFunc({ arg msg;
		Synth(\lppclick)
	}, '/lpp', nil);
});

s.boot;
