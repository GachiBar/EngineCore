using System;
using System.Collections;
using System.Collections.Generic;

namespace GameplayCore.Collections
{
    public class PriorityQueue<TElement, TPriority> : IEnumerable<(TElement, TPriority)>, IEnumerable
        where TPriority : IComparable
    {
        private List<(TElement Item, TPriority Priority)> _items = new List<(TElement Item, TPriority Priority)>();

        public int Count => _items.Count;

        public IEnumerator GetEnumerator()
        {
            return GetEnumerator();
        }

        IEnumerator<(TElement, TPriority)> IEnumerable<(TElement, TPriority)>.GetEnumerator()
        {
            return _items.GetEnumerator();
        }

        public void Clear()
        {
            _items.Clear();
        }

        public void Enqueue(TElement item, TPriority priority)
        {
            int index = 0;

            while (true)
            {
                if (index == _items.Count || _items[index].Priority.CompareTo(priority) < 0)
                {
                    _items.Insert(index, (item, priority));
                    break;
                }

                index += 1;
            }
        }

        public TElement Dequeue()
        {
            var item = _items[0].Item;
            _items.Remove(_items[0]);
            return item;
        }

        public TElement Peek()
        {
            return _items[0].Item;
        }
    }
}
