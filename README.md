# Ample
Ample is my attempt at a drum machine (sequencer, sampler, eventual sound mangler) based mostly on the Digitakt series. 
Right now it's pretty simple. It can play 8 tracks of samples with 8 steps each.
It is written in C++ using the JUCE framework. Getting to the point where it can trigger on/off (short) arbitrary samples was a big
stepping stone. 

Now there's a lot of plumbing internally to clean up and then more features to add. I will be adding an effects chain
so that each track (maybe each step) will be individually modifiable. Planned effects are reverb, delay, compression, and distortion.
There may be a few more effects if possible. A tentative to-do list follows.

# TO DO
Effects
- [ ] Reverb
- [ ] Delay
- [ ] Compression
- [ ] Distortion

Code Quality
- [ ] Tests
- [ ] Integrate the JUCE ValueTree
- [ ] Clarity and Structure

General
- [ ] Per step editing of samples
- [ ] Serialization and saving track state
- [ ] Some concept of a set and project file
- [ ] Port to Raspberry Pi
- [ ] Build hardware (Another rabbit hole)
- [ ] Fancy and nice UI
