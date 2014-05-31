Hi graders!

Learned a lot from this assignment - it's by far the biggest piece of code I've ever written.  There are several things I'd fix / do differently if I had more time.

My AI uses board simulations.  I tried to inherit the BoardSim class directly from Board.  I think it would've been better to define an interface from which they both inherited.  

As it is, there's a lot of duplicated/almost duplicated code, because Board functions don't seem to use dynamic dispatch when calling virtual Board methods.  Possibly there's a way to do this, but, in a hurry, I ended up copy pasting :P.

Second, I'm not impressed with the quality of play.  I tried to be clever about reusing my BoardSims (the way suggested in the lectures would end up being quartic time if I understand it correctly; mine is 'only' quadratic).  

I don't know how good the recommended method is, and again I unfortunately don't have time to try it.  Even if you don't have time to look at the code in detail, I hope some of you will play a game and tell me how it compares.

On the plus side, it works, it's reasonably fast for medium-sized boards, and I think I'm starting to grok memory management in C++.

Thanks for your time, and I look forward to your feedback.
