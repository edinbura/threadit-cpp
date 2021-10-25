# threadit-cpp
<h1 style="font-size:130%;"> A C++ library to support the active object concept. </h1>

Class CThreadIt supports the asynchronous model of an active object that can accept request to do work within a thread of control and return the
result of the work processing. Work requests are accepted and stored in a buffer until the thread is able to perform the work associated with
the request. Completed work requests (the result of the work) are buffered until the requestor is able to retrieve the result. 
The model requires the following classes:
<p>
CWorkPackIt &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- &nbsp; This class describes the work to be performed.<br>
CWorkPackIt &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- &nbsp; This class describes the results of the work.<br>
CThreadIt &nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp; &nbsp;- &nbsp; Implements the model that defines how clients are able to execute work in separate thread. <br>
Derived Class &nbsp;- &nbsp; Defines the processing to perform in response to work requests.<br>
</p>                
The class derived from CThreadIt creates work packages using CWorkPackIt. 
These work packages are associated with a given method that the class implements. When the CThreadIt class gets the work package it will invoke
the method in it's own thread of control. The method implementation takes the input values from the work packages and peforms the necessary
processing. When finished the results are compiled and placed in a given output queue. The next work package in the input queue is
retrieved. The model also includes asynchronous notification to the requestor when work is completed. By following simple rules it is
very easy for an application to utilise active objects using the model defined here.

As an example, class CThreadIt can be used to provide a separate thread of execution to perform processing on behalf of clients with a processing
completion notification being sent to the client. This would find use when a User Interface must perform a lengthy process that may tie up
the screen response. The processing may be delegated to an instance derived from this class for execution in separate thread thus not
impacting on the user response. When work is complete the User Interface is notified and retrieves the work done from the work output queue.

Another use case on the server is to accept incoming requests with the ability to check the incoming queue size and then take action if the 
processing is taking to long and the request queue is increasing in size.
