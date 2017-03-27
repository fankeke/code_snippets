class Solution(object):
    def helper(self,left,right):
        if left is None and right is None:
            return True
        if left is None or right is None or left.val!=right.val:
            return False
        return self.helper(left.left,right.right) and self.helper(left.right,right.left)
        
    def isSymmetric(self, root):
        """
        :type root: TreeNode
        :rtype: bool
        """
        if root is None:
            return True
        return self.helper(root.left,root.right)
