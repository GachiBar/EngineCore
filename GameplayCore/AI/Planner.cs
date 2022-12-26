using GameplayCore.Collections;
using System.Collections.Generic;
using System.Xml.Linq;

namespace GameplayCore.AI
{
    public class Planner
    {
        private class Node
        {
            public int Cost;
            public State State;
            public Node Parent;
            public Action Action;
        }

        public Plan MakePlan(State state, Goal goal, List<Action> actions)
        {
            var queue = new PriorityQueue<Node, int>();
            var visited = new List<Node>();

            var initial = new Node();
            var current = initial;
            initial.Cost = 0;
            initial.State = state.Copy();
            initial.Parent = null;
            initial.Action = null;

            queue.Enqueue(initial, 0);

            while (queue.Count > 0)
            {
                current = queue.Dequeue();

                if (goal.DistanceTo(current.State) == 0)
                {
                    break;
                }

                foreach (var action in actions)
                {
                    if (action.CheckPreconditions(current.State))
                    {
                        var neighbour = new Node();                       
                        neighbour.Cost = current.Cost + 1;
                        neighbour.State = current.State.Copy();                        
                        neighbour.Parent = current;
                        neighbour.Action = action;
                        action.PlanEffects(neighbour.State);

                        bool isShouldMarkAsVisited = true;

                        foreach (var node in visited)
                        {
                            if (node.State.IsSame(neighbour.State))
                            {
                                isShouldMarkAsVisited = false;

                                if (node.Cost > neighbour.Cost)
                                {
                                    var priority = goal.DistanceTo(node.State);
                                    node.Cost = neighbour.Cost;
                                    node.Parent = neighbour.Parent;
                                    node.Action = neighbour.Action;
                                    queue.Enqueue(node, node.Cost + priority);
                                }
                            }
                        }

                        if (isShouldMarkAsVisited)
                        {
                            var priority = goal.DistanceTo(neighbour.State);
                            visited.Add(neighbour);
                            queue.Enqueue(neighbour, neighbour.Cost + priority);
                        }
                    }
                }
            }

            var planActions = new List<Action>();

            for (var temp = current; temp.Parent != null; temp = temp.Parent)
            {
                planActions.Insert(0, temp.Action);
            }
            
            return new Plan(planActions);
        }
    }
}
