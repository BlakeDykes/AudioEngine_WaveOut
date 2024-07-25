## Demo of stitched .wav Playback
https://youtu.be/MtMV9OQYQg8  

The Jetson's theme song is split into 23 separate .wav files of varying sizes (128k - 512k). Waves are loaded asynchronously using an A/B buffer and passed to the audio thread, where they are queued and played using WaveOut.
