import speech_recognition as sr
r = sr.Recognizer()
harvard = sr.AudioFile('dog.wav')
with harvard as source:
    audio = r.record(source)
text = r.recognize_google(audio)
print(text)

print(type(text))

if (text.find('go to the dog house')!= -1):
    print("1")
if(text.find('follow me')!= -1):
    print("2")
