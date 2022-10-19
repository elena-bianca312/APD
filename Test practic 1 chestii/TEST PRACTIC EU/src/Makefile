JFLAGS = -g
JC = javac

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
		  Testapd.java \
		  Task.java \
		  Initiator.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
