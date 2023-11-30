import speech_recognition as sr
import append_audio
state = 0
def rec(state):

    append_audio.combine()
    r = sr.Recognizer()
    harvard = sr.AudioFile('c:/Users/danie/Documents/College Stuff/Senior Year/7th Semester (Fall)/ENEE408I/Python/sounds.wav')
    with harvard as source:
        audio = r.record(source)
    text = r.recognize_google(audio)
    print(text)

    print(type(text))

    
    if (text.find('go')!= -1 or text.find('to')!= -1 or text.find('the')!= -1 or text.find('the')!= -1 or text.find('dog')!= -1 or text.find('house')!= -1):
        print("1")
        state = 1
    if(text.find('follow')!= -1 or text.find('me')!= -1):
        print("2")
        state = 2
    """"
    if(text.find('stop')):
        print("3")
        state = 3
    """
    
    return state