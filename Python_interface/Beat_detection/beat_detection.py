import sys
from pathlib import Path
from os.path import realpath, exists
from os import listdir
from typing import Union, List
from scipy.io.wavfile import read
from numpy import array, median, float16, float32, float64, ndarray
from librosa.beat import beat_track
from librosa.onset import onset_strength
from librosa import load

# Check if converted file already exists if yes dont convert it

def convert_file(sound_file: str, dtype: type = float32, hop_length: int = 512) -> Union[ndarray]:
    """Converts the given wav or mp3 file to array of beat timing
     and saves the array csv or byte file.
     
     ### Args:
        - sound_file (str): Path to wav or mp3 to be converted.
        - destination (str): The path to the destination file.
        - dtype (type, Optional): The type of float to cast the array contense as.
        uses np.flats {float16, float32, float64}.
    ### Returns:
        - np.array: The array of numpy floats {float16, float32, float64} of beat timings in ms.
     """
    if sound_file[-3:] == "wav":
        sample_rate, data = read(sound_file)
        data = array([x[0] for x in data], dtype=float)
    else:
        data, sample_rate = load(sound_file)
        data = array(data, dtype=float)
    onset_env = onset_strength(y=array(data, dtype=float), sr=sample_rate, hop_length=hop_length, aggregate=median, 
                               lag=1, # Time lag for computing differences
                               max_size=1, # default no filtering of frequ bins
                               detrend=False, # default, do notfilter onset strength to remove DC component
                               center=True) # Centered frame analysis in STFT, by hop length
    bpm, beat_timing = beat_track(onset_envelope=onset_env, sr=sample_rate, units="time", hop_length=512, tightness=7)
    return array(beat_timing, dtype=dtype)*1000

def output_file(output_file_path: str, output_array: Union[List[float], ndarray], file_extention: str):
    """"""
    if file_extention == "bin":
        file = open(output_file_path, "wb")
        file.write(bytearray(output_array))
        file.close()
    else:
        file = open(output_file_path, "w")
        for nb in output_array:
                file.write(str((str(nb)+"\n").encode("ascii")))
        file.close()

path: Path = Path(realpath(__file__)) / ".."
"""The path to the directory where this script is present. (works on all comupters)"""
file_extention: str = "txt"
"""The extention to append to the end of the file path."""
arg_file_name: Union[str, None] = None
"""The name of the file to convert passed as script argument."""
# Parameter handeling
if len(sys.argv) >= 2:
    for arg in sys.argv[1:2]:
        if arg == "-b":
            file_extention = "bin"
        elif not exists(path / "Sound_files" / arg):
            print("One of the given agruments is not a valid path!")
            sys.exit([1])
        else:
            arg_file_name = arg
# If a file name is given convert only that file
if arg_file_name:
    new_file_name: str = arg_file_name[:-4].__add__(f"_{arg_file_name[-3:]}.{file_extention}")
    print(f"Converting file: \"{arg_file_name}\"")
    print(path.joinpath("Sound_files").joinpath(arg_file_name).parts[-1])
    output_file(path / "Timing_arrays" / new_file_name,
                convert_file(path.joinpath("Sound_files").joinpath(arg_file_name).parts[-1]),
                file_extention)
    print(f"File \"{arg_file_name}\" converted.")
# If no specific file name is passed convert the whole contense of the Sound_files folder
else:
    output_folder_files: List[str] = [f for f in listdir(path / "Timing_arrays") if ".txt" in f or ".bin" in f]
    for file_name in [f for f in listdir(path / "Sound_files") if ".wav" in f or ".mp3" in f]:
        new_file_name: str = file_name[:-4].__add__(f"_{file_name[-3:]}.{file_extention}")
        new_file_path: Path = path.joinpath("Timing_arrays").joinpath(new_file_name)
        print(f"Part: {new_file_path.parts[-1]}, new file: {new_file_name}")
        if new_file_path.parts[-1] not in output_folder_files:
            print(f"Converting file: {file_name}")
            output_file(new_file_path, 
                        convert_file(path.joinpath("Sound_files").joinpath(file_name).as_posix()), 
                        file_extention)
        else:
            print(f"File \"{file_name}\" alredy present in output folder.")
    print("All convertions finished!")
# For binary files
#convert_file(path+"/../Sound_files/"+file).tofile(path+"/../Timing_arrays/"+file)

    #if isdir(arg1):
    #    for file in [f for f in listdir(path+"/../Sound_files") if ".wav" in f or ".mp3" in f]:
    #        convert_file(path+"/../Sound_files/"+file).tofile(sys.argv[2])
    #else:
    #    
    #    convert_file(path+"/../Sound_files/"+arg1.split("/")[-1]).tofile(sys.argv[2])
