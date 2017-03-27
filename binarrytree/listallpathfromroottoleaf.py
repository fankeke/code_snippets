
class TreeNode:
    def __init__(self,val):
	self.val=val
	self.left,self.right=None,None

class Solution:
    def binaryTreePaths(self,root):
	result=[]
	if root is None:
	    return result;
	self.dfs(root,result,[]);
	return result


    def dfs(self,node,reslut,tmp):
	tmp.append(str(node.val));
	if node.left is None and node.right is None:
	    result.append('->'.join(tmp));
	    tmp.pop()
    	    return
	if node.left:
	    self.dfs(self.left,reslut,tmp)
        if node.right:
	    self.dfs(self.right,result,tmp)
        tmp.pop()
