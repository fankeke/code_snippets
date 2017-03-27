
# also adopt this module
#   dfs(node,tmp,...):
#	tmp.append(node)
#    	if node.left is None and node.right is None:
#		xxx
#		tmp.pop()
#    		return
#	if node.left:
#		dfs(node.left,tmp,...)
#    	if node.right:
#		dfs(node.right,tmp,...)
#    	tmp.pop()
    
class Solution(object):
    def getpath(self,node,path,p):
        if node is None:
            return False
        path.append(node)
        if node==p:
            return True
        if node.left and self.getpath(node.left,path,p) or node.right and self.getpath(node.right,path,p):
            return True
        path.pop()
        return False
        
    def printpath(self,name,path):
        print "\n %s \n" % name
        for item in path:
            print item.val,
        print "\n"
        
    def lowestCommonAncestor(self, root, p, q):
        """
        :type root: TreeNode
        :type p: TreeNode
        :type q: TreeNode
        :rtype: TreeNode
        """
        pathp,pathq=[],[]
        self.getpath(root,pathp,p)
        self.getpath(root,pathq,q);
        self.printpath("pathp",pathp)
        self.printpath("pathq",pathq)
        lenp,lenq=len(pathp),len(pathq)
        ans,x=None,0
        while x<min(lenp,lenq) and pathp[x]==pathq[x]:
            ans,x=pathp[x],x+1
        return ans   
