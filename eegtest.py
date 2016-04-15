import mne
mne.set_log_level('INFO')
raw = mne.io.read_raw_brainvision('/Volumes/Untitled 14/Facebook Piot Experiment/EEG/test2.vhdr')
print(raw.info)
print(raw.ch_names)
raw.plot()
