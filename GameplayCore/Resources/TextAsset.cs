using System;
using System.IO;
using System.Text;

namespace GameplayCore.Resources
{
    public class TextAsset : Resource
    {
        public TextAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }

        string GetText(bool readBigFiles = false)
        {
            FileStream file = File.Open(FilePath, FileMode.Open, FileAccess.Read, FileShare.Read);
            long fileSize = readBigFiles ? file.Length : Math.Min(1024, file.Length);
            
            if (!readBigFiles && file.Length > 1024)
            {
                Console.WriteLine("It's to big file, so filesize was clamped due to (readBigFiles = false)!");
            }
            
            byte[] buf = new byte[fileSize];
            int readen = file.Read(buf, 0, buf.Length);
            string content = Encoding.UTF8.GetString(buf, 0, readen);

            file.Close();
            return content;
        }
    }
}