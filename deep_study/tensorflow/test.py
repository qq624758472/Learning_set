class Dog:
    def __init__(self, petname, temp):
        self.name = petname;
        self.temperature = temp;

    def status(self):
        print("dog name is ", self.name)
        print("dog temperature is ", self.temperature)
        pass

    def setTemperature(self, temp):
        self.temperature = temp;
        pass

    def bark(self):
        print("woof!")
        pass

    pass


lassie = Dog("Lassie", 37)
print(lassie.status())
lassie.setTemperature(40)
print(lassie.status())
