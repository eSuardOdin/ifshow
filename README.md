# ifshow
[School Project] - Getting network interfaces infos
Ok ?


## Compilation :
<u>Pour mon hôte et VYOS :</u>
- **ifshow**:   ``gcc ifshow/main.c if_functions/if_functions.c -o bin/ifshow``
- **agent**:   ``gcc ifnetshow/server/main.c ifnetshow/server/agent/agent.c  if_functions/if_functions.c -o bin/test_agent``
- **client**:   ``gcc ifnetshow/client/client_functions/client_functions.c ifnetshow/client/main.c -o bin/test_client``

<u>Pour mon alpine :</u>
- **ifshow**:   ``musl-gcc ifshow/main.c if_functions/if_functions.c -o bin/alpine/ifshow``
- **agent**:   ``musl-gcc ifnetshow/server/main.c ifnetshow/server/agent/agent.c  if_functions/if_functions.c -o bin/alpine/agent_ifnetshow``
- **client**:   ``musl-gcc ifnetshow/client/client_functions/client_functions.c ifnetshow/client/main.c -o bin/alpine/client_ifnetshow``

<u>Pour mes tinycores :</u>
- **ifshow**:   ``gcc ifshow/main.c if_functions/if_functions.c -o bin/alpine/ifshow -m32``
- **agent**:   ``musl-gcc ifnetshow/server/main.c ifnetshow/server/agent/agent.c  if_functions/if_functions.c -o bin/alpine/agent_ifnetshow``
- **client**:   ``musl-gcc ifnetshow/client/client_functions/client_functions.c ifnetshow/client/main.c -o bin/alpine/client_ifnetshow``
