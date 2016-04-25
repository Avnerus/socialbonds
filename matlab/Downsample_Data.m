%% RESAMPLING OF  DATA
srate=512;                                     % define your downsampled rate
EEG = pop_resample( EEG, srate);   %resample the data at sampling rate defined, srate.
[ALLEEG EEG CURRENTSET] = pop_newset(ALLEEG, EEG, CURRENTSET,'setname','dowsampled_dataset,'gui','off'); % create a new dataset for your resampled data
EEG = eeg_checkset( EEG );
EEG = pop_saveset( EEG, 'filename','dowsampled_dataset','filepath',currDir);   % currDir = definition of the directory in which you wish to save your processed data
eeglab redraw