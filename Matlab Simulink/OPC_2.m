function [x] = OPC_2(y)
% variables
persistent init_Server;
persistent init_Nodes;
persistent uaClient;
persistent Var_Node_In1;
persistent Var_Node_In2;
persistent Var_Node_Out;
persistent testVal;
% initialize variables
if (isempty(init_Server))
    testVal = 0;
    init_Server = 0;
end

% OPC UA server (PLC) address and connecting client (Simulink) to the
% server
if init_Server == 0
   init_Server = 1;
    uaClient = opcua('192.168.3.2',4840);                
    connect(uaClient);                                    
end

% write variables of the server
if uaClient.isConnected == 1

    % wrute in the variables of the OPC UA server
    Var_Node_Out= opcuanode(3,'"OpcInterface"."Feedback"',uaClient);    
    
    % assign input y of the function to "Feedback" variable
    writeValue(uaClient, Var_Node_Out, y);                  

end

% read variables of the server
if uaClient.isConnected == 1
    
    % read out the variables of the OPC UA server
    Var_Node_In1 = opcuanode(3,'"OpcInterface"."Controlinput"."xv"',uaClient);
    Var_Node_In2 = opcuanode(3,'"OpcInterface"."Controlinput"."fx"',uaClient);
    
    % read "Controlinput" value from server and store in "val"
    val1 = readValue(uaClient, Var_Node_In1);
    val2 = readValue(uaClient, Var_Node_In2);
               
    % assign "val" to variable "testVal"
    testVal = [val1 val2];
end

% assign "ControlInput and Thickness setpoint" ("testVal") value to the output x of the function
x = double(testVal);
end