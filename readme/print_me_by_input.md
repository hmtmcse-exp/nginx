#### How to add Print Me Module? 
 1. [Module here](https://github.com/hmtmcse/nginx/tree/master/module/Print_Me_By_Input)
 2. Add module at compilation
  
  --add-module=/path/nginx/module/Print_Me_By_Input
 
 3. Use in config file
 
 ```bash
        location / {
            print_me_by_input "Yes I Able to get Input.  Ha ha ha ha";    
        }
 ```
 
 