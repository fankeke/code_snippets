class Solution(object):
    def getleftdepth(self,node):
        if node is None:
            return 0
        ans=1
        while node.left:
            ans+=1
            node=node.left
        return ans
    def getrightdepth(self,node):
        if node is None:
            return 0
        ans=1
        while node.right:
            ans+=1
            node=node.right
        return ans
        
    def countNodes(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        if root is None:
            return 0
        leftdepth=self.getleftdepth(root.left)+1
        rightdepth=self.getrightdepth(root.right)+1
        print leftdepth,rightdepth
        if leftdepth==rightdepth:
            return (1<<leftdepth)-1
        else:
            return 1+self.countNodes(root.left)+self.countNodes(root.right)
