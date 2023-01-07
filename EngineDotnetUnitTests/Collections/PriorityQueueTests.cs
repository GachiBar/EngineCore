using GameplayCore.Collections;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;

namespace EngineDotnetUnitTests.Collections
{
    public class PriorityQueueTests
    {
        [Test]
        public void Enqueue_SequenceOfIntegers_IntegersHasRightOrder()
        {
            var values = new int[] { 1, 4, 5, 6, 2, 8, 1, 7 };
            var queue = new PriorityQueue<int, int>();
            var serviced = new List<int>();

            foreach (var value in values)
            {
                queue.Enqueue(value, value);
            }

            Array.Sort(values);

            while (queue.Count > 0)
            {
                serviced.Add(queue.Dequeue());
            }

            Assert.IsTrue(values.SequenceEqual(serviced));
        }
    }
}
