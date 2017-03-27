
#in python,list and dict could changed as paramenter,but int,string could't,
# so I used a list[0] as a int to change the ans in function....


class Solution(object):
    def sumNumbers(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        tmp=[]
        ans=[0]			#use list[0] as a int to change it ,,so pathetic...
        if root is None:
            return 0
        self.dfs(root,tmp,ans)
        return ans[0]
        
    def getsum(self,tmp):
        s=0
        for item in tmp:
            s=s*10+item;
        return s
        
    def dfs(self,node,tmp,ans):
        tmp.append(node.val)
        if node.left is None and node.right is None:
            ans[0]+=self.getsum(tmp)
            tmp.pop()
            return
        if node.left:
            self.dfs(node.left,tmp,ans)
        if node.right:
            self.dfs(node.right,tmp,ans)
        tmp.pop()
        
