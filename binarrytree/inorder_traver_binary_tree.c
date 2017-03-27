//p point to root;




//my code: (wrong)

do{
        while(p){
            stack_push(s,p);
            p=p->left;
        }
        p=stack_pop(s);
        ans[count++]=p->val;
        p=p->right;
  }while(!stack_empty(s));                   
  
  
  
  
//right  code:

do{
      while(p){
          stack_push(s,p);
          p=p->left;
       }
    
      p=stack_pop(s);
      ans[count++]=p->val;
      p=p->right;
    
  }while(!stack_empty(s)||p);               //it's important to add the condition p in "while" or it will end unsuccesful.
  
  
//or it can be like this: pretty much clean and understandable.

while(p||!stack_empty(p)){
      while(p){
        stack_push(s,p);
        p=p->left;
      }
      p=stack_pop(s);
      ans[count++]=p->val;
      p=p->right;
}
  
  
  
  
  
  
  
