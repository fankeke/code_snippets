#every node has not value ,eg :every node value is 1
class Solution(object):
    def maxpath(self,root):
	ans=[0]
	self.helper(root,ans)
	return ans[0]

    def helper(self,node,ans):        #return maxpathlen that node as root
	if node is None:
	    return 0
	leftlen=helper(node.left,ans)
	rightlen=helper(node.right,ans)

	pathsum=leftlen+rightlen+1

	ans[0]=max(ans[0],pathsum)
    	
    	return pathsum



#every node has a value
class Solution(object):
    def maxPathSum(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        ans=[-999999]
        self.helper(root,ans)
        return ans[0]
    
    def helper(self,root,ans):
        if root is None:
            return 0
        leftpath=self.helper(root.left,ans)
        rightpath=self.helper(root.right,ans)
        
        leftrightpath=leftpath+rightpath+root.val
        
        singlepath=max(max(leftpath,rightpath)+root.val,root.val)
        ans[0]=max(ans[0],max(leftrightpath,singlepath))
            
        return singlepath
