# from root to leaf path ,the sum is a given sum


class Solution(object):
    def pathSum(self, root, sum):
        """
        :type root: TreeNode
        :type sum: int
        :rtype: List[List[int]]
        """
        result=[]
        if root is None:
            return result
        self.dfs(root,result,[],sum)
        #print "result:",result
        return result
        
    def getsum(self,tmp):
        s=0
        for item in tmp:
            s+=item
        #print "sum is:%d" % s
        return s
        
    def dfs(self,node,result,tmp,sum):
        tmp.append(node.val)
        if node.left is None and node.right is None:
            if self.getsum(tmp)==sum:
                ans=copy.copy(tmp)
                result.append(ans)        #防止浅拷贝，即直接result.append(tm)，
    					#这样只是押入了其引用，而没有生成真正的数组对象
            tmp.pop()
            return
        
        if node.left:
            self.dfs(node.left,result,tmp,sum)
        if node.right:
            self.dfs(node.right,result,tmp,sum)
        tmp.pop()
