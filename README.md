This is an experimental cognitive architecture for multi-agent simulations.
The idea is to be able to support the creation and maintenance of narratives
which may be exchanged, retold, mutated, negated and so on and to move towards
a situation where agents posess a socially constructed identity and are
capable of self reflection.

Monkeymind is not intended to simulate any particular primate cognition in detail.  It's an abstract model.

Some guiding assumptions are:

 * The brain is not a homogeneous lump of jelly.  It has discernable architecture.
 * An important part of being a primate, or any large brained creature which lives in social groups, is about keeping track of the state of other individuals upon whom your survival may depend.
 * In primates with advanced language features egocentric communication becomes internalised such that the thought process consists partially of an inner dialog (silent speech, Vygotsky et al).
 * Human-like cognition is strongly based around episodic memories and their arrangement into narrative structure.
 * Narratives represent a compact and memorable way of communicating both meaning and intent such that shared understanding may be established.
 * Shared knowledge of the state of other individuals means that it's possible to coordinate better as a group, facilitating group survival (eg. as hunters or as a team whose members perform different roles).
 * Obtaining interesting cognitive behaviors requres the generation of a dense psychological jungle.  For a social creature, navigating the psychological or sociological landscape of competing desires and social status is as important as navigating the physical landscape.
 * Identity arrises from the mechanisms of social coordination.  It is socially constructed.
 * The ultimate arbiter of intelligence is survival, not utility.  Many behaviors characteristic of primates are not really fully explicable in utilitarian terms.


Abstract Persons
----------------

Other individuals who are met are categorised using a self-origanising map, and maps may be partially aligned during conversation.  Friend or foe, or attraction values are then indexed against the 2D map.

Installing
----------
sudo apt-get install build-essential libz-dev

mm_object
---------
This is the most fundamental structure which represents a general object which can contain a number of properties.

mm_id
-----
A unique identifier which can be applied to mm_objects. It's just a random string.

mm_tale
-------
This structure describes a story as a sequence of events (represented as mm_objects).

mm_narratives
-------------
Can contain a number of stories represented by mm_tale structures

mm_episodic
-----------
This is similar to mm_tale in that it stores a sequence of events, but it's a ring buffer. The idea is that this represents the current working memory, which lay later have sub-sequences be extracted as mm_tale. The current episocid buffer can also be confabulated with previous mm_tales (stored within mm_narratives) in order to produce framing effects and the possibility of prediction.

Confabulation as the core of cognition
--------------------------------------
An underlying principle is one of confabulation being the main underlying operation mechanism of the cognitive system. You could also call this "pattern matching" or "analogy making" or "pattern completion". Sequences of events can be matched in an inexact way such that unknowns may be filled in or knowns overwritten by ones which have stronger narrative popularity within the culture. Inexact pattern matching makes it possible to understand the present via framing and to predict the future to some extent, or to superimpose fictional narratives onto observed ones to create heroic identities or a mythologised self.

Confabulation-besed cognition also has similarity to replacement automata, and it is expected that such systems can exhibit Wolfram-like effects, including Turing completeness. Complex effects such as Turing machines and Wolfram class 3 or 4 systems are predicted to be observed at the intersections of cultures, with cultures themselves being metastable.
