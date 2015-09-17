/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode(int x) { val = x; }
 * }
 */
public class Solution {
    public ListNode rotateRight(ListNode head, int k) {
        if(head == null || head.next==null || k == 0)
            return head;
        int len = 1;
        ListNode fast = head;
        while(fast.next != null){
            fast = fast.next;
            len++;
        }
        k = k%len;
        if(k == 0)  
            return head;
        
        int move = len - k - 1;
        ListNode slow = head;
        while(move > 0){
            slow = slow.next;
            move--;
        }

        ListNode newhead = slow.next;
        slow.next = null;
        fast.next = head;
        return newhead;
        
    }
}
