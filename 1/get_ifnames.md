# Populate the table to be displayed

We are going to iterate through all addresses got with getifaddrs()

LOOP BEGING
    IF address ifname not already in tab (return a pointer to it or NULL):
        IF Options is -i and name does not match :
            Continue;
        ELSE
            Add new struct if_info to tab and return a pointer to it (pointer that should have been set with first tab check)
    
    Add address to appropriate addresses pointer of struct if_info
    (passing it to a function like : add_address(struct sock_addr ad, if_info *if))
    