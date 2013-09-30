This is an experimental cognitive architecture for multi-agent simulations.
The idea is to be able to support the creation and maintenance of narratives
which may be exchanged, retold, mutated, negated and so on and to move towards
a situation where agents posess a socially constructed identity and are
capable of self reflection.

Monkeymind is not intended to simulate any particular primate cognition in detail.  It's an obstract model.

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

Property Matrix
---------------

Each agent can have a number of peoperties.  When two agents meet the coocurrence of properties is plotted within a 2D matrix, and this is used as the basis for forming stereotypes.  Each point in the matrix is incremented according to whether the met agent is judged to be friendly or unfriendly.  Negative values in the matrix correspond to feature correspondences for probably hostile agents whereas positive values correspond to the feature correspondences of friendly agents.
